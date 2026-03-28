"use client";

import dynamic from "next/dynamic";

type ChartPoint = {
    date: string;
    close: number;
};

const StockChart = dynamic(() => import("./StockChart"), {
    ssr: false,
    loading: () => <p>Loading chart...</p>,
});

export default function StockChartClient({ data }: { data: ChartPoint[] }) {
    return <StockChart data={data} />;
}
