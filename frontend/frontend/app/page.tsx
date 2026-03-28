import { execFileSync } from "node:child_process";
import path from "node:path";
import StockChartClient from "./stock-chart-client";

type SearchParams = {
  start?: string;
  end?: string;
  ticker?: string;
};

type QueryResult = {
  runtimeNs: string;
  headers: string[];
  rows: string[][];
};

function formatDateForInput(date: Date): string {
  const year = date.getFullYear();
  const month = String(date.getMonth() + 1).padStart(2, "0");
  const day = String(date.getDate()).padStart(2, "0");
  return `${year}-${month}-${day}`;
}

function parseOutput(stdout: string): QueryResult {
  const lines = stdout
    .split(/\r?\n/)
    .map((line) => line.trim())
    .filter((line) => line.length > 0);

  if (lines.length < 2) {
    throw new Error("Unexpected output from out binary.");
  }

  const runtimeNs = lines[0];
  const headers = lines[1].split(",");
  const rows = lines.slice(2).map((line) => line.split(","));

  return { runtimeNs, headers, rows };
}

function runQuery(
  dsType: "RB" | "BP",
  ticker: string,
  start: string,
  end: string,
): QueryResult {
  const projectRoot = path.resolve(process.cwd(), "..", "..");
  const binaryPath = path.join(projectRoot, "out");

  const stdout = execFileSync(binaryPath, [dsType, ticker, start, end], {
    cwd: projectRoot,
    encoding: "utf-8",
  });

  return parseOutput(stdout);
}

export default async function Home({
  searchParams,
}: {
  searchParams: Promise<SearchParams>;
}) {
  const params = await searchParams;
  const defaultStart = "2000-01-01";
  const defaultEnd = formatDateForInput(new Date());
  const start = params.start ?? defaultStart;
  const end = params.end ?? defaultEnd;
  const ticker = params.ticker ?? "AAPL";

  let rbResult: QueryResult | null = null;
  let bpResult: QueryResult | null = null;

  rbResult = runQuery("RB", ticker, start, end);
  bpResult = runQuery("BP", ticker, start, end);

  const displayResult = rbResult ?? bpResult;
  const dateIndex = displayResult ? displayResult.headers.indexOf("date") : -1;
  const closeIndex = displayResult ? displayResult.headers.indexOf("close") : -1;
  const pointCount = displayResult ? displayResult.rows.length : 0;
  const chartData =
    displayResult && dateIndex >= 0 && closeIndex >= 0
      ? displayResult.rows
        .map((row) => ({
          date: row[dateIndex],
          close: Number(row[closeIndex]),
        }))
        .filter((point) => Number.isFinite(point.close))
      : [];

  return (
    <main>
      <h1>Lake Wauburg Trading Company</h1>

      <form method="GET">
        <fieldset>
          <legend>Query</legend>

          <table
            style={{
              border: "2px solid black",
              borderCollapse: "collapse",
            }}
          >
            <tbody>
              <tr style={{ border: "1px solid black" }}>
                <td style={{ border: "1px solid black", padding: "6px" }}>
                  <label htmlFor="ticker">Ticker</label>
                </td>
                <td style={{ border: "1px solid black", padding: "6px" }}>
                  <select id="ticker" name="ticker" defaultValue={ticker}>
                    <option value="AAPL">AAPL</option>
                    <option value="MSFT">MSFT</option>
                    <option value="TSLA">TSLA</option>
                  </select>
                </td>
              </tr>
              <tr style={{ border: "1px solid black" }}>
                <td style={{ border: "1px solid black", padding: "6px" }}>
                  <label htmlFor="start">Start date</label>
                </td>
                <td style={{ border: "1px solid black", padding: "6px" }}>
                  <input id="start" name="start" type="date" defaultValue={start} required />
                </td>
              </tr>
              <tr style={{ border: "1px solid black" }}>
                <td style={{ border: "1px solid black", padding: "6px" }}>
                  <label htmlFor="end">End date</label>
                </td>
                <td style={{ border: "1px solid black", padding: "6px" }}>
                  <input id="end" name="end" type="date" defaultValue={end} required />
                </td>
              </tr>
              <tr style={{ border: "1px solid black" }}>
                <td colSpan={2} style={{ border: "1px solid black", padding: "6px" }}>
                  <button type="submit">Run Query (1000 times)</button>
                </td>
              </tr>
            </tbody>
          </table>
        </fieldset>
      </form>

      {rbResult || bpResult ? (
        <section>
          <table
            style={{
              border: "2px solid black",
              borderCollapse: "separate",
              borderSpacing: "8px",
            }}
          >
            <tbody>
              <tr>
                <th style={{ fontSize: "16px", border: "1px solid black", padding: "12px" }}>
                  Red-Black Tree Time (ns, median of 1000)
                </th>
                <td
                  style={{
                    fontSize: "36px",
                    border: "1px solid black",
                    padding: "12px",
                    fontFamily: "monospace",
                  }}
                >
                  {rbResult ? rbResult.runtimeNs : "N/A"}
                </td>
              </tr>
              <tr>
                <th style={{ fontSize: "16px", border: "1px solid black", padding: "12px" }}>
                  B+ Tree Time (ns, median of 1000)
                </th>
                <td
                  style={{
                    fontSize: "36px",
                    border: "1px solid black",
                    padding: "12px",
                    fontFamily: "monospace",
                  }}
                >
                  {bpResult ? bpResult.runtimeNs : "N/A"}
                </td>
              </tr>
              <tr>
                <th style={{ fontSize: "16px", border: "1px solid black", padding: "12px" }}>
                  Points
                </th>
                <td
                  style={{
                    fontSize: "36px",
                    border: "1px solid black",
                    padding: "12px",
                    fontFamily: "monospace",
                  }}
                >
                  {pointCount}
                </td>
              </tr>
            </tbody>
          </table>
        </section>
      ) : null}

      {displayResult ? (
        <section>
          <StockChartClient data={chartData} />
        </section>
      ) : null}
    </main>
  );
}
